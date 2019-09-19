#include "NECAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


NECAnalyzerSettings::NECAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mPreTimeMark(9000),
	mPreTimeSpace(4500),
	mMark(560),
	mOneSpace(1600),
	mZeroSpace(560)
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "NEC", "Infrared NEC Analyzer" );
	mInputChannelInterface->SetChannel( mInputChannel );

	mPreTimeMarkInterface.reset(new AnalyzerSettingInterfaceInteger());
	mPreTimeMarkInterface->SetTitleAndTooltip("Pre-Time Mark(us)", "AGC mark duration");
	mPreTimeMarkInterface->SetMax(100000);
	mPreTimeMarkInterface->SetMin(200);
	mPreTimeMarkInterface->SetInteger(9000);

	mPreTimeSpaceInterface.reset(new AnalyzerSettingInterfaceInteger());
	mPreTimeSpaceInterface->SetTitleAndTooltip("Pre-Time Space(us)", "AGC space duration");
	mPreTimeSpaceInterface->SetMax(100000);
	mPreTimeSpaceInterface->SetMin(200);
	mPreTimeSpaceInterface->SetInteger(4500);

	mMarkInterface.reset(new AnalyzerSettingInterfaceInteger());
	mMarkInterface->SetTitleAndTooltip("Mark(us)", "Mark duration");
	mMarkInterface->SetMax(100000);
	mMarkInterface->SetMin(200);
	mMarkInterface->SetInteger(560);

	mOneSpaceInterface.reset(new AnalyzerSettingInterfaceInteger());
	mOneSpaceInterface->SetTitleAndTooltip("One Space(us)", "One space duration");
	mOneSpaceInterface->SetMax(100000);
	mOneSpaceInterface->SetMin(200);
	mOneSpaceInterface->SetInteger(1600);

	mZeroSpaceInterface.reset(new AnalyzerSettingInterfaceInteger());
	mZeroSpaceInterface->SetTitleAndTooltip("Zero Space(us)", "Zero space duration");
	mZeroSpaceInterface->SetMax(100000);
	mZeroSpaceInterface->SetMin(200);
	mZeroSpaceInterface->SetInteger(560);

	AddInterface( mInputChannelInterface.get() );
	AddInterface(mPreTimeMarkInterface.get());
	AddInterface(mPreTimeSpaceInterface.get());
	AddInterface(mMarkInterface.get());
	AddInterface(mOneSpaceInterface.get());
	AddInterface(mZeroSpaceInterface.get());


	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "NEC", false );
}

NECAnalyzerSettings::~NECAnalyzerSettings()
{
}

bool NECAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mPreTimeMark = mPreTimeMarkInterface->GetInteger();
	mPreTimeSpace = mPreTimeSpaceInterface->GetInteger();
	mMark = mMarkInterface->GetInteger();
	mOneSpace = mOneSpaceInterface->GetInteger();
	mZeroSpace = mZeroSpaceInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannel, "NEC Analyzer", true );

	return true;
}

void NECAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
	mPreTimeMarkInterface->SetInteger(mPreTimeMark);
	mPreTimeSpaceInterface->SetInteger(mPreTimeSpace);
	mMarkInterface->SetInteger(mMark);
	mOneSpaceInterface->SetInteger(mOneSpace);
	mZeroSpaceInterface->SetInteger(mZeroSpace);
}

void NECAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mPreTimeMark;
	text_archive >> mPreTimeSpace;
	text_archive >> mMark;
	text_archive >> mOneSpace;
	text_archive >> mZeroSpace;

	ClearChannels();
	AddChannel( mInputChannel, "NEC Analyzer", true );

	UpdateInterfacesFromSettings();
}

const char* NECAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mPreTimeMark;
	text_archive << mPreTimeSpace;
	text_archive << mMark;
	text_archive << mOneSpace;
	text_archive << mZeroSpace;

	return SetReturnString( text_archive.GetString() );
}
