#ifndef NEC_SIMULATION_DATA_GENERATOR
#define NEC_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class NECAnalyzerSettings;

class NECSimulationDataGenerator
{
public:
	NECSimulationDataGenerator();
	~NECSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, NECAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	NECAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:

	U64 UsToSamples(U64 us);
	U64 SamplesToUs(U64 samples);

	void Delay(U64 ms);
	void AGC_PulseWrite();
	void AdressWrite();
	void NotAdressWrite();
	void CommandWrite();
	void NotCommandWrite();

	void WriteBit(U8 bit);

	U64 mT;
	U8 mAdress;
	U8 mData;

	SimulationChannelDescriptor mNECSimulationData;

};
#endif //NEC_SIMULATION_DATA_GENERATOR