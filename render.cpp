#include <Bela.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/OscReceiver/OscReceiver.h>

OscReceiver oscReceiver;
OscSender oscSender;

// TODO support avahi/zeroconf/bonjour _osc.tcp.local
struct {
	const char* address = "0.0.0.0";  // Actually, this is hardwired.
	int port = 58000;
} receiver;
struct {
	const char* address = "192.168.1.168";
	int port = 9000;
} sender;
int flag_message_received = 0;

const char* address[4] = { "/1/fader1", "/1/fader2", "/1/fader3", "/1/fader4" };
float value[4] = { 0.0, 0.0, 0.0, 0.0 };

void on_receive(oscpkt::Message* msg, const char *endpoint, void* arg)
{
	const char* in_address = msg->addressPattern().c_str();
	printf("RECEIVE | ENDPOINT(%s) ADDRESS(%s)\n", endpoint, in_address);
	if (!strcmp(in_address, address[0])) {
		msg->match(address[0]).popFloat(value[0]);
		flag_message_received = 1;
	}
}

bool setup(BelaContext *context, void *userData)
{
	printf("Starting up.\n");
	
	oscReceiver.setup(receiver.port, on_receive);
	oscSender.setup(sender.port, sender.address);

	printf("Receiving on port %s:%d .\n", receiver.address, receiver.port);
	printf("Sending to port %s:%d .\n", sender.address, sender.port);
	printf("Sender should be TouchOSC Mk2 using layout 'Simple MK2'.\n");
	printf("Reading %s only.\n", address[0]);
	printf("OSC initialized.\n");

	return true;
}

void render(BelaContext *context, void *userData)
{
	
	if (flag_message_received) {
		printf("%s %f\n", address[0], value[0]);
		flag_message_received = 0;
	}
	// oscSender.newMessage(address[1]).add(value[1]).send();
}

void cleanup(BelaContext *context, void *userData)
{
	printf("Shutting down.\n");
}

/**
OSC example adapted to test Hexler TouchOSC (Mk2).
Only a receiver so far.  See the code for ports and layouts and addresses.
**/

