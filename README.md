# NEC Analyzer

Saleae Logic Analyzer for the NEC IR-Remote Protocol.

* add protocol to your Salae Logic:<br>
for x64: <br>
copy `\Binaries\x64\NECAnalyzer.dll` to `\Program Files\Saleae Inc\Analyzers`<br>
for x86: <br>
copy `\Binaries\x86\NECAnalyzer.dll` to `\Program Files\Saleae Inc\Analyzers`<br>
and restart Salae Logic Software
* changing protocol:<br>
open MS Visio project `\VisualStudio\NECAnalyzer.sln`

* Saleae: https://www.saleae.com
* Saleae Logic: https://www.saleae.com/downloads
* Analyzer SDK: https://support.saleae.com/saleae-api-and-sdk/protocol-analyzer-sdk

## NEC Protocol Description
Standart NEC protocol consist of preambule (AGC pulse), adress, inverted adress, command and inverted command.
![](./Screenshots/NECMessageFrame.png)

Standart timings are:<br>
AGC pulse mark - 9ms<br>
AGC pulse space - 4.5ms<br>
Bit Mark - 560us<br>
Space for One - 1.6ms<br>
Space for Zero - 560us<br>
![](./Screenshots/NECBitsTiming.png)

But there is many modification of this protocol. Each company change timings for itself. For this reason there is settings for timings of protocol.
![](./Screenshots/ProtocolSettings.png)

Protocol Simulation use standart timings for NEC protocol.
![](./Screenshots/ProtocolSimulation.png)

Based on Salae Protocol Analyzer SDK version: 1.2.17

