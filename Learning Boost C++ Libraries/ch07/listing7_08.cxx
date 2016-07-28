 1 class MessageHandler
 2 {
 3 public:
 4   ...
 5   void listenOnQueue(Queue& q, MessageType msgType) {
 6     q.listen(msgType, [this](Message msg) 
 7                       { handleMsg(msg); } );
 8   }
 9 
10   void handleMsg(Message msg) { ... }
11 };

