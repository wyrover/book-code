-- Does a "dictionary order" less-than comparison:
function DictCmp(A, B)
  -- "Fold" each argument by deleting all non-alphanumeric
  -- characters then lowercasing it:
  local FoldedA = string.lower(string.gsub(A, "%W+", ""))
  local FoldedB = string.lower(string.gsub(B, "%W+", ""))
  -- Only compare the folded versions if they're unequal:
  if FoldedA ~= FoldedB then
    return FoldedA < FoldedB
  else
    return A < B
  end
end
