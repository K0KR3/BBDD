SELECT
    o.country,
    COUNT(o.officecode) as no_ventas
FROM offices o
WHERE o.officecode NOT IN(
    SELECT DISTINCT e.officecode
    FROM employees e 
    JOIN customers c
        ON c.salesrepemployeenumber = e.employeenumber
    JOIN orders o
        ON o.customernumber = c.customernumber 


)
GROUP BY
    o.country
ORDER BY 
    no_ventas DESC;