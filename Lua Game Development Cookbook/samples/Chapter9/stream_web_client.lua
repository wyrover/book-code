local zmq = require 'zmq'
require 'utils/pstring'

local context = assert(zmq.context())
local socket = assert(context.socket(zmq.ZMQ_STREAM))
socket.options.stream_notify = true
socket.options.ipv6 = true
assert(socket.connect("tcp://www.google.com:80"))
local identity = socket.options.identity
local once = false

local poll = zmq.poll {
	{socket, zmq.ZMQ_POLLIN, function(socket)
		local id = assert(socket.recv())
		local data = assert(socket.recv())
		if #data>0 then
			print("%q\n%q" % {zmq.tohex(id), data})
		end

		if not once then
			assert(socket.send(id, zmq.ZMQ_SNDMORE))
			assert(socket.send([[
GET / HTTP/1.1

]], zmq.ZMQ_SNDMORE))
			once = true
		end
	end},
}

while true do
	poll.start()
end
socket.disconnect()
