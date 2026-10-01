SELECT 
    e.employeenumber,
    e.lastname
FROM employees e
JOIN employees director
    ON e.reportsto = director.employeenumber
WHERE director.employeenumber = (
    SELECT employeenumber
    FROM employees
    WHERE reportsto IS NULL
)
