-- The first version:
SELECT Ord.Id, CustId, NameLast, ProductId, DescStr, Count
FROM Cust, Ord, Product
WHERE Cust.Id = Ord.CustId
  AND Ord.ProductId = Product.Id;

-- The version with AS:
SELECT Ord.Id AS OrdId, CustId, NameLast, ProductId, DescStr, Count
FROM Cust, Ord, Product
WHERE Cust.Id = Ord.CustId
  AND Ord.ProductId = Product.Id;
