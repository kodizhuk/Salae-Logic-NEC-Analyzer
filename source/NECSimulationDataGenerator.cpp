#include "NECSimulationDataGenerator.h"
#include "NECAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

NECSimulationDataGenerator::NECSimulationDataGenerator()
	:mAdress(0x0f),
	mData(0x87)
{
}

NECSimulationDataGenerator::~NECSimulationDataGenerator()
{
}

void NECSimulationDataGenerator::Initialize( U32 simulation_sample_rate, NECAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mNECSimulationData.SetChannel( mSettings->mInputChannel );
	mNECSimulationData.SetSampleRate( simulation_sample_rate );
	mNECSimulationData.SetInitialBitState( BIT_HIGH );

	mT = UsToSamples(10);	//10us
}

U32 NECSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mNECSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		Delay(10);
		AGC_PulseWrite();
		AdressWrite();
		NotAdressWrite();
		CommandWrite();
		NotCommandWrite();
		Delay(30);
	}

	*simulation_channel = &mNECSimulationData;
	return 1;
}

U64 NECSimulationDataGenerator::UsToSamples(U64 us)
{
	return (mSimulationSampleRateHz * us) / 1000000;
}

U64 NECSimulationDataGenerator::SamplesToUs(U64 samples)
{
	return(samples * 1000000) / mSimulationSampleRateHz;
}

void NECSimulationDataGenerator::Delay(U64 ms)
{
	mNECSimulationData.Advance(U32(mT * ms * 100));	//delay
}

void NECSimulationDataGenerator::AGC_PulseWrite()
{
	mNECSimulationData.TransitionIfNeeded(BIT_LOW);
	mNECSimulationData.Advance(U32(mT * 900));
	mNECSimulationData.TransitionIfNeeded(BIT_HIGH);
	mNECSimulationData.Advance(U32(mT * 450));
}

void NECSimulationDataGenerator::AdressWrite()
{
	//00001111
	//msb  lsb
	//lsb must be first -> 1111000
	U8 data = mAdress;
	for (U32 i = 0; i < 8; ++i)
		WriteBit((data >> i) & 0x01);
}

void NECSimulationDataGenerator::NotAdressWrite()
{
	U8 data = ~mAdress;
	for (U32 i = 0; i < 8; ++i)
		WriteBit((data >> i) & 0x01);
}

void NECSimulationDataGenerator::CommandWrite()
{
	U8 data = mData;
	for (U32 i = 0; i < 8; ++i)
		WriteBit((data >> i) & 0x01);
}

void NECSimulationDataGenerator::NotCommandWrite()
{
	U8 data = ~mData;
	for (U32 i = 0; i < 8; ++i)
		WriteBit((data >> i) & 0x01);
	mNECSimulationData.TransitionIfNeeded(BIT_LOW);
	mNECSimulationData.Advance(U32(mT * 56));
	mNECSimulationData.TransitionIfNeeded(BIT_HIGH);
}

void NECSimulationDataGenerator::WriteBit(U8 bit)
{
	mNECSimulationData.TransitionIfNeeded(BIT_LOW);
	mNECSimulationData.Advance(U32(mT * 56));
	mNECSimulationData.TransitionIfNeeded(BIT_HIGH);
	if (bit)
		mNECSimulationData.Advance(U32(mT * 56 * 3));
	else
		mNECSimulationData.Advance(U32(mT * 56));
	
}