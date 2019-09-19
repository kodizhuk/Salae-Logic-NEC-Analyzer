#ifndef NEC_ANALYZER_H
#define NEC_ANALYZER_H

#include <Analyzer.h>
#include "NECAnalyzerResults.h"
#include "NECSimulationDataGenerator.h"

class NECAnalyzerSettings;
class ANALYZER_EXPORT NECAnalyzer : public Analyzer2
{
public:
	NECAnalyzer();
	virtual ~NECAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	U64 UsToSample(U64 us);
	U64 SamplesToUs(U64 samples);

	std::auto_ptr< NECAnalyzerSettings > mSettings;
	std::auto_ptr< NECAnalyzerResults > mResults;
	AnalyzerChannelData* mNEC;

	NECSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//NEC analysis vars:
	U32 mSampleRateHz;
	U32 mTAGCMark;
	U32 mTAGCSpace;
	U32 mTMark;
	U32 mTSpace0;
	U32 mTSpace1;
	U32 mTError;
	bool mSynchronised;
	std::vector<bool> mBitsForNextByte; //value
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //NEC_ANALYZER_H
