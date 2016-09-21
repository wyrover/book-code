local req = [[
	local name = unpack(arg)
	local socket,msg = assert(context.socket(zmq.ZMQ_REQ))
	local result, msg = assert(socket.connect("inproc://test1"))
	local poll = zmq.poll()

	poll.add(socket, zmq.ZMQ_POLLIN, function(socket)
		local result = socket.recvAll()
		if result then
			print(name, string.format("Recieved data: %q", result))
		end
	end)

	socket.send("Lorem ipsum")

	poll.start()
	socket.close()
]]

local zmq = require 'zmq'
local context, msg = assert(zmq.context())
local socket,msg = assert(context.socket(zmq.ZMQ_REP))
local result, msg = assert(socket.bind("inproc://test1"))
local poll = zmq.poll()
	
poll.add(socket, zmq.ZMQ_POLLIN, function(socket)
	local result = socket.recvAll()
	if result then
		print("Reply part",
		string.format("Recieved data: %q", result))
		socket.send(result.." dolor sit amet")
	end
end)

local thread = context.thread(req, 'Request part')
poll.start()

thread.join()
socket.close()

