#include <cpunit>
using namespace cpunit;

#include <string>
using namespace std;
#include <Socket.h>

namespace TimeOutTest 
{
    CPUNIT_TEST(TimeOutTest, test_1_second) {
        string toSend = "some string for testing";

        Socket receiver("1000", TCP, toBind);
        receiver.Listen(1);
        
        Socket sender("1000", TCP, toConnect);
        receiver.Accept();
        sender.Send(toSend);

        assert_equals(toSend, receiver.Receive(1));
    }
}
