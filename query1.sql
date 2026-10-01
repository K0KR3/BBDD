SELECT 
    c.customernumber,
    c.customername,
    SUM(p.amount) AS total
FROM customers c
JOIN payments p
    ON c.customernumber = p.customernumber
WHERE c.customernumber IN(
    SELECT o.customernumber
    FROM orders o
    JOIN orderdetails od
        ON od.ordernumber = o.ordernumber
    JOIN products pr
        ON pr.productcode = od.productcode
    WHERE productname = '1940 Ford Pickup Truck'
)
GROUP BY 
    c.customernumber,
    c.customername
ORDER BY
    total DESC;
        
