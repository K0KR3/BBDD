SELECT 
    e.officecode, 
    SUM(od.quantityordered) AS productos_vendidos
FROM 
    public.orderdetails od
JOIN 
    public.orders o ON od.ordernumber = o.ordernumber
JOIN 
    public.customers c ON o.customernumber = c.customernumber
JOIN 
    public.employees e ON c.salesrepemployeenumber = e.employeenumber
GROUP BY 
    e.officecode
ORDER BY 
    productos_vendidos DESC
LIMIT 1;