/**
OSC example adapted to test Hexler TouchOSC.
**/

#include <Bela.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/OscReceiver/OscReceiver.h>

OscReceiver oscReceiver;
OscSender oscSender;

// TODO support avahi/zeroconf/bonjour _osc.tcp.local
int receivePort = 58000;
const char* sendAddress = "192.168.1.168";
int sendPort = 9000;

const char* fader1_key = "/pager1/1/fader1";
const char* fader2_key = "/pager1/1/fader2";
float fader1 = 0.5;
float fader2 = 0.0;

void on_receive(oscpkt::Message* msg, const char *sender, void* arg)
{
	printf("%s %s\n", sender, msg->addressPattern().c_str());
	msg->match(fader1_key).popFloat(fader1);
	msg->match(fader2_key).popFloat(fader2);
	printf("faders = %f, %f\n", fader1, fader2);
}

bool setup(BelaContext *context, void *userData)
{
	printf("Starting up.\n");
	
	oscReceiver.setup(receivePort, on_receive);
	oscSender.setup(sendPort, sendAddress);

	printf("Receiving on port %d .\n", receivePort);
	printf("Sending to port %s:%d .\n", sendAddress, sendPort);
	printf("OSC initialized.\n");
	printf("Sender should be TouchOSC Mk2 using layout 'Simple MK2'.\n");
	
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
	printf("Shutting down.\n");
}
