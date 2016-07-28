 1 MessageHandler *handler = new MessageHandler(...);
 2 Queue q(...);
 3 ...
 4 q.listen(msgType, [handler](Message msg)
 5                   {  handler->handleMessage(msg);  }
 6                   );

