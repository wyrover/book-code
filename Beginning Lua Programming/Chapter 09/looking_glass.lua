local function KnightFnc()
  print [[Knight:
The name of the song is called "HADDOCKS' EYES."
]]
  coroutine.yield()
  print [[Knight, looking a little vexed:
No, you don't understand, that's what the name is CALLED. The
name really IS "THE AGED AGED MAN."
]]
  coroutine.yield()
  print [[Knight:
No, you oughtn't, that's quite another thing! The SONG is
called "WAYS AND MEANS," but that's only what it's CALLED, you
know!
]]
  coroutine.yield()
  print [[Knight:
I was coming to that. The song really IS "A-SITTING ON A GATE",
and the tune's my own invention.
]]
end

local function Alice()
  local Knight = coroutine.create(KnightFnc)

  coroutine.resume(Knight)
  print [[Alice, trying to feel interested:
Oh, that's the name of the song, is it?
]]
  coroutine.resume(Knight)
  print [[Alice:
Then I ought to have said "That's what the SONG is called"?
]]
  coroutine.resume(Knight)
  print [[Alice, completely bewildered:
Well, what IS the song, then?
]]
  coroutine.resume(Knight)
end

Alice()
