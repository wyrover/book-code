function Answer(Question)
  local Res
  if Question == "no bananas" then
    Res = "yes"
  elseif Question == "everything" then
    Res = 42
  elseif Question == "Tuesday" then
    Res = "Belgium"
  else
    error("No answer for " .. tostring(Question))
  end
  return Res
end

print(Answer("this statement is false"))
