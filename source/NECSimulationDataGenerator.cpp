#include "NECSimulationDataGenerator.h"
#include "NECAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

NECSimulationDataGenerator::NECSimulationDataGenerator()
:	mSerialText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
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
}

U32 NECSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mNECSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		mNECSimulationData.Advance(1000);
		mNECSimulationData.Transition();
		/*
		AGC_PulseWrite();
		AdressWrite();
		NotAdressWrite();
		CommandWrite();
		NotCommandWrite();
		*/
	}

	*simulation_channel = &mNECSimulationData;
	return 1;
}

void NECSimulationDataGenerator::AGC_PulseWrite()
{

}

void NECSimulationDataGenerator::AdressWrite()
{}

void NECSimulationDataGenerator::NotAdressWrite()
{}

void NECSimulationDataGenerator::CommandWrite()
{}

void NECSimulationDataGenerator::NotCommandWrite()
{}