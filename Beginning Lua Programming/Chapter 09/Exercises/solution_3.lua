local function LclConsumer(Evt)
  print("Consumer: initialize")
  while Evt ~= "end" do
    print("Consumer: " .. Evt)
    Evt = coroutine.yield()
  end
  print("Consumer: finalize")
end

local function LclProducer()
  print("Producer: initialize")
  local PutEvent = coroutine.wrap(LclConsumer)
  -- Simulate event generation
  local List = {"mouse", "keyboard", "keyboard", "mouse"}
  for J, Val in ipairs(List) do
    local Evt = string.format("Event %d (%s)", J, Val)
    print("Producer: " .. Evt)
    PutEvent(Evt)
  end
  PutEvent("end")
  print("Producer: finalize")
end

LclProducer()
