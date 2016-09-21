local zmq = require "zmq"
local context = assert(zmq.context())
local socket = assert(context.socket(zmq.ZMQ_REQ))
assert(socket.connect("tcp://localhost:12345"))

local len = assert(socket.send("Test message"))
if len and len > 0 then
  local poll = zmq.poll()
  poll.add(socket, zmq.ZMQ_POLLIN, function(s)
    print(assert(socket.recvAll()))
  end)
  poll.start()
end
socket.disconnect()
