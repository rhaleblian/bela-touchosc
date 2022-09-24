/**
OSC example adapted to test Hexler TouchOSC (Mk2).
Only a receiver so far.  See the code for ports and layouts and addresses.

2020-2021 yoyodyne research
**/

#include <Bela.h>
#include <libraries/OscSender/OscSender.h>
#include <libraries/OscReceiver/OscReceiver.h>
#include <MiscUtilities.h>
#include <iostream>
#include <stdlib.h>

OscReceiver oscReceiver;
OscSender oscSender;

// TODO look at supporting avahi/zeroconf/bonjour _osc.tcp.local
struct {
	const char* address = "0.0.0.0";  // Actually, this is implied/implicit.
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

bool isPepper(void) {
	auto var = getenv("HOME");
	if (var)
		std::cout << "HOME=" << var << std::endl;
	var = getenv("PATH");
	if (var)
		std::cout << "PATH=" << var << std::endl;
		
	auto pepper = getenv("PEPPER");
	if (pepper) {
		std::cout << "I'm a PEPPER, you're a PEPPER, everyone wants to be a PEPPER too" << std::endl;
		return true;
	} else {
		std::cout << "I'm not a PEPPER, you should consider it." << std::endl;
		return false;
	}
}

bool setup(BelaContext *context, void *userData)
{
	std::cout << "Starting up." << std::endl;

	isPepper();
	
	oscReceiver.setup(receiver.port, on_receive);
	oscSender.setup(sender.port, sender.address);

	std::cout << "Receiving on port" << receiver.address << ":" << receiver.port << " ." << std::endl;
	std::cout << "Sending to port" << sender.address << ":" << sender.port << " ." << std::endl;
	std::cout << "Sender should be TouchOSC Mk2 using layout 'Simple MK2'." << std::endl;
	std::cout << "Reading " << address[0] << " only." << std::endl;
	std::cout << "OSC initialized." << std::endl;

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
