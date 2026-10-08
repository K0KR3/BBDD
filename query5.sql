SELECT o.country, COUNT(*) AS no_ventas
FROM offices o
WHERE NOT EXISTS (
    SELECT 1
    FROM employees e
    JOIN customers c 
        ON c.salesrepemployeenumber = e.employeenumber
    JOIN orders od
        ON od.customernumber = c.customernumber
    WHERE e.officecode = o.officecode
        AND od.orderdate >= '2003-01-01'
        AND od.orderdate <  '2004-01-01'
)
GROUP BY o.country
ORDER BY no_ventas DESC;
