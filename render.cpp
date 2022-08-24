/**
OSC example adapted to test Hexler TouchOSC.
**/

#include <Bela.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/OscReceiver/OscReceiver.h>

OscReceiver oscReceiver;
OscSender oscSender;

// TODO support zeroconf/bonjour
int localPort = 59588;
int remotePort = 9000;
const char* remoteIp = "192.168.1.168";
float fader1 = 0.5;
float fader2 = 0.0;

bool handshakeReceived;
void on_receive(oscpkt::Message* msg, const char *what, void* arg)
{
	printf("%s %s\n", msg->addressPattern().c_str(), what);
	msg->match("/3/fader2").popFloat(fader2);
	printf("faders = %f, %f\n", fader1, fader2);
}

bool setup(BelaContext *context, void *userData)
{
	oscReceiver.setup(localPort, on_receive);
	oscSender.setup(remotePort, remoteIp);

	oscSender.newMessage("/3/fader1").add(0.25f).send();
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
	oscSender.newMessage("/3/fader1").add(fader1).send();
}

void cleanup(BelaContext *context, void *userData)
{

}
