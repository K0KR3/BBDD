SELECT e.employeenumber, e.lastname
FROM employees e
JOIN employees jefe
    ON e.reportsto = jefe.employeenumber
JOIN employees director
    ON jefe.reportsto = director.employeenumber
WHERE director.reportsto IS NULL;
