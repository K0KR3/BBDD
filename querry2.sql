SELECT 
    pr.productline, 
    AVG(o.shippeddate - o.orderdate) AS tiempo_medio
FROM 
    public.orders o
JOIN 
    public.orderdetails od ON o.ordernumber = od.ordernumber
JOIN 
    public.products pr ON od.productcode = pr.productcode
GROUP BY 
    pr.productline;