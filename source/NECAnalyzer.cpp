#include "NECAnalyzer.h"
#include "NECAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

NECAnalyzer::NECAnalyzer()
:	Analyzer2(),  
	mSettings( new NECAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

NECAnalyzer::~NECAnalyzer()
{
	KillThread();
}

void NECAnalyzer::SetupResults()
{
	mResults.reset( new NECAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void NECAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mNEC = GetAnalyzerChannelData( mSettings->mInputChannel );

	if( mNEC->GetBitState() == BIT_LOW )
		mNEC->AdvanceToNextEdge();

	//init parametrs
	mTAGCMark = mSettings->mPreTimeMark;
	mTAGCSpace = mSettings->mPreTimeSpace;
	mTMark = mSettings->mMark;
	mTSpace0 = mSettings->mZeroSpace;
	mTSpace1 = mSettings->mOneSpace;
	mTError = 100;
	mSynchronised = false;

	mBitsForNextByte.clear();

	U64 edge_location = 0, frame_start_location, frame_end_location;
	for( ; ; )
	{
		//find first AGC pulse
		mNEC->AdvanceToNextEdge();	//falling
		if (mNEC->GetBitState() == BIT_LOW)
		{
			edge_location = mNEC->GetSampleNumber();
		}

		U64 next_edge_location = mNEC->GetSampleOfNextEdge();
		U64 edge_distance = next_edge_location - edge_location;
		if ((edge_distance < UsToSample((U64)mTAGCMark + mTError)) && (edge_distance > UsToSample(mTAGCMark - mTError)))
		{
			mNEC->AdvanceToNextEdge();	//rising

			next_edge_location = mNEC->GetSampleOfNextEdge();
			edge_distance = next_edge_location - edge_location;
			if ((edge_distance < UsToSample((U64)mTAGCMark + mTAGCSpace + mTError))
				&& (edge_distance > UsToSample((U64)mTAGCMark + mTAGCSpace - mTError)))
			{
				mSynchronised = true;
				frame_start_location = next_edge_location;
				mResults->AddMarker(edge_location, AnalyzerResults::Start, mSettings->mInputChannel);
				mResults->AddMarker(next_edge_location, AnalyzerResults::Stop, mSettings->mInputChannel);
				mResults->CommitResults();
				ReportProgress(mNEC->GetSampleNumber());
			}
		}
		//receive adress
		//receive command
		while (mSynchronised == true)
		{
			CheckIfThreadShouldExit();
			mNEC->AdvanceToNextEdge();

			edge_location = mNEC->GetSampleNumber();
			next_edge_location = mNEC->GetSampleOfNextEdge();
			edge_distance = next_edge_location - edge_location;
			//find mark
			if ((edge_distance < UsToSample((U64)mTMark + mTError)) && (edge_distance > UsToSample(mTMark - mTError)))
			{
				//find space
				mNEC->AdvanceToNextEdge();
				edge_location = mNEC->GetSampleNumber();
				next_edge_location = mNEC->GetSampleOfNextEdge();
				edge_distance = next_edge_location - edge_location;
				if ((edge_distance < UsToSample((U64)mTSpace0 + mTError)) && (edge_distance > UsToSample(mTSpace0 - mTError)))
					mBitsForNextByte.push_back(0);
				else if ((edge_distance < UsToSample((U64)mTSpace1 + mTError)) && (edge_distance > UsToSample(mTSpace1 - mTError)))
					mBitsForNextByte.push_back(1);
				else
				{
					//error 
					mSynchronised = false;
					mBitsForNextByte.clear();
				}
			}
			else
			{
				//error read data
				mSynchronised = false;
				mBitsForNextByte.clear();
			}

			if (mBitsForNextByte.size() == 8)
			{
				//finish read adress, push data to frame
				frame_end_location = next_edge_location;

				U64 byte = 0;
				for (U32 i = 0; i < 8; ++i)
					byte |= (mBitsForNextByte[i] << i);
				mBitsForNextByte.clear();

				Frame frame;
				frame.mStartingSampleInclusive = frame_start_location;
				frame.mEndingSampleInclusive = frame_end_location;
				frame.mData1 = byte;
				mResults->AddFrame(frame);
				mResults->CommitResults();
				ReportProgress(mNEC->GetSampleNumber());

				frame_start_location = frame_end_location;
			}
		}

		ReportProgress(mNEC->GetSampleNumber());
		CheckIfThreadShouldExit();
	}
}

U64 NECAnalyzer::UsToSample(U64 us)
{
	return (mSampleRateHz * us) / 1000000;
}

U64 NECAnalyzer::SamplesToUs(U64 samples)
{
	return(samples * 1000000) / mSampleRateHz;
}

bool NECAnalyzer::NeedsRerun()
{
	return false;
}

U32 NECAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 NECAnalyzer::GetMinimumSampleRateHz()
{
	return 3500;	//3.5kHz
}

const char* NECAnalyzer::GetAnalyzerName() const
{
	return "NEC Analyzer";
}

const char* GetAnalyzerName()
{
	return "NEC Analyzer";
}

Analyzer* CreateAnalyzer()
{
	return new NECAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}