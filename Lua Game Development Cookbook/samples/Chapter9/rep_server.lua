local zmq = require 'zmq'

local context = assert(zmq.context())
local socket = assert(context.socket(zmq.ZMQ_REP))

assert(socket.bind('tcp://*:12345'))

local poll = zmq.poll()

poll.add(socket, zmq.ZMQ_POLLIN, function(socket)
	local result = assert(socket.recvAll())
	print('Received a message: '..result)
	assert(socket.send('This is a reply to: '..result))
end)

local timeout = 500 -- 500ms
while (true) do
	poll.start(timeout)
end

socket.close()
