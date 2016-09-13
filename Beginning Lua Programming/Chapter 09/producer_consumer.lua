local function LclProducer()
  print("Producer: initialize")
  -- Simulate event generation
  local List = {"mouse", "keyboard", "keyboard", "mouse"}
  for J, Val in ipairs(List) do
    local Evt = string.format("Event %d (%s)", J, Val)
    print("Producer: " .. Evt)
    coroutine.yield(Evt)
  end
  print("Producer: finalize")
  return "end"
end

local function LclConsumer()
  local GetEvent = coroutine.wrap(LclProducer)
  local Evt
  print("Consumer: initialize")
  while Evt ~= "end" do
    Evt = GetEvent()
    print("Consumer: " .. Evt)
  end
  print("Consumer: finalize")
end

LclConsumer()
