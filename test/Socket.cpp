#include <cpunit>
using namespace cpunit;

#include <string>
#include <thread>
using namespace std;
#include <Socket.h>

namespace TimeOutTest 
{
    CPUNIT_TEST(TimeOutTest, test_1_second) {
        string toSend = "some string for testing", whatIReceived;

        Socket receiver("1000", TCP, toBind);
        receiver.Listen(1);
        Socket sender("1000", TCP, toConnect);
        
        try {
            thread([&]{
                whatIReceived = receiver.Accept().Receive(1);
            });
            thread([&]{
                sender.Send(toSend);
            }).join();
        } catch(...) {
            assert_true("caught exception", false);
        }

        assert_equals(toSend, whatIReceived);
    }
}
