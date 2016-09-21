local zmq = require 'zmq'
local context = assert(zmq.context())
local socket = assert(context.socket(zmq.ZMQ_SUB))
assert(socket.connect("tcp://localhost:12345"))

local topic = 'demo'
socket.options.subscribe = topic

local poll = zmq.poll()

poll.add(socket, zmq.ZMQ_POLLIN, function(s)
	local topic = assert(socket.recv())
	local result = assert(socket.recvAll())
	print(topic, result)
end)

while (true) do
	poll.start()
end
	
socket.disconnect()


