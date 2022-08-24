/**
OSC example adapted to test Hexler TouchOSC.
**/

#include <Bela.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/OscReceiver/OscReceiver.h>

OscReceiver oscReceiver;
OscSender oscSender;

// TODO support zeroconf/bonjour
int localPort = 8000;
int remotePort = 9000;
const char* remoteIp = "192.168.1.168";
// bool handshakeReceived;

float fader1 = 0.5;
float fader2 = 0.0;

void on_receive(oscpkt::Message* msg, const char *what, void* arg)
{
	printf("%s %s\n", msg->addressPattern().c_str(), what);
	msg->match("/pager1/1/fader1").popFloat(fader1);
	msg->match("/pager1/1/fader2").popFloat(fader2);
	printf("faders = %f, %f\n", fader1, fader2);
}

bool setup(BelaContext *context, void *userData)
{
	oscReceiver.setup(localPort, on_receive);
	oscSender.setup(remotePort, remoteIp);

	printf("OSC initialized. Remote port is is %s:%d .\n", remoteIp, remotePort);
	printf("Use layout 'Simple MK2'.\n");
	
	// oscSender.newMessage("/3/fader1").add(0.25f).send();
	return true;
}

void render(BelaContext *context, void *userData)
{
	float v = 0.01;
	if (fader1 > 0.75)
		v = -0.01;
	if (fader1 < 0.25)
		v = 0.01;
	fader1 += v;
	// oscSender.newMessage("/3/fader1").add(fader1).send();
}

void cleanup(BelaContext *context, void *userData)
{

}
