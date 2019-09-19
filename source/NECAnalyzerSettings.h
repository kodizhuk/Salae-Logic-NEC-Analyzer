#ifndef NEC_ANALYZER_SETTINGS
#define NEC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class NECAnalyzerSettings : public AnalyzerSettings
{
public:
	NECAnalyzerSettings();
	virtual ~NECAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //NEC_ANALYZER_SETTINGS
