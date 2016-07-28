 1 class MessageHandler
 2 {
 3 public:
 4   ...
 5   void listenOnQueue(Queue& q, MessageType msgType) {
 6     q.listen(msgType, boost::bind(&MessageHandler::handleMsg,
 7                                   this, _1));
 8   }
 9 
10   void handleMsg(Message msg) { ... }
11 };

