local zmq = require 'zmq'
require 'utils/pstring'

local context = assert(zmq.context())
local socket = assert(context.socket(zmq.ZMQ_STREAM))
socket.options.stream_notify = true
socket.options.ipv6 = true
assert(socket.bind("tcp://*:80"))
--local identity = socket.options.identity

local poll = zmq.poll {
	{socket, zmq.ZMQ_POLLIN, function(socket)
		local identity = socket.options.identity
		local id = assert(socket.recv())
		local data = assert(socket.recv())
		if #data>0 then
			assert(socket.send(id, zmq.ZMQ_SNDMORE))
			assert(socket.send([[
HTTP/1.0 200 OK
Content-Type: text/plain

Hello, World!]], zmq.ZMQ_SNDMORE))

			assert(socket.send(id, zmq.ZMQ_SNDMORE))
			assert(socket.send("", zmq.ZMQ_SNDMORE))
		end
	end},
}

while true do
	poll.start()
end
socket.disconnect()
