SELECT 
    od1.productcode AS producto_1, 
    od2.productcode AS producto_2, 
    COUNT(od1.ordernumber) AS numero_carros
FROM 
    public.orderdetails od1
JOIN 
    public.orderdetails od2 ON od1.ordernumber = od2.ordernumber 
                           AND od1.productcode < od2.productcode
GROUP BY 
    od1.productcode, 
    od2.productcode
HAVING 
    COUNT(od1.ordernumber) > 1;