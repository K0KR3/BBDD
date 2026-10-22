DROP TABLE IF EXISTS customer_representatives CASCADE;
DROP TABLE IF EXISTS employee_offices CASCADE;
DROP TABLE IF EXISTS payments CASCADE;
DROP TABLE IF EXISTS orderdetails CASCADE;
DROP TABLE IF EXISTS orders CASCADE;
DROP TABLE IF EXISTS products CASCADE;
DROP TABLE IF EXISTS productlines CASCADE;
DROP TABLE IF EXISTS customers CASCADE;
DROP TABLE IF EXISTS employees CASCADE;
DROP TABLE IF EXISTS offices CASCADE;

CREATE TABLE public.customers (
    customernumber integer NOT NULL,
    customername character varying(50) NOT NULL,
    contactlastname character varying(50) NOT NULL,
    contactfirstname character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    city character varying(50) NOT NULL,
    state character varying(50) DEFAULT NULL::character varying,
    postalcode character varying(15) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    salesrepemployeenumber integer,
    creditlimit numeric(10,2) DEFAULT NULL::numeric
);

CREATE TABLE public.employees (
    employeenumber integer NOT NULL,
    lastname character varying(50) NOT NULL,
    firstname character varying(50) NOT NULL,
    extension character varying(10) NOT NULL,
    email character varying(100) NOT NULL,
    officecode character varying(10) NOT NULL,
    reportsto integer,
    jobtitle character varying(50) NOT NULL,
    CONSTRAINT employees_pkey PRIMARY KEY (employeenumber),
    CONSTRAINT employees_fk_reportsto FOREIGN KEY (reportsto) 
        REFERENCES employees(employeenumber) ON DELETE SET NULL
);

CREATE TABLE public.offices (
    officecode character varying(10) NOT NULL,
    city character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    state character varying(50) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    postalcode character varying(15) NOT NULL,
    territory character varying(10) NOT NULL,
    CONSTRAINT offices_pkey PRIMARY KEY (officecode)
);

CREATE TABLE public.orderdetails (
    ordernumber integer NOT NULL,
    productcode character varying(15) NOT NULL,
    quantityordered integer NOT NULL,
    priceeach numeric(10,2) NOT NULL,
    orderlinenumber smallint NOT NULL
);

CREATE TABLE public.orders (
    ordernumber integer NOT NULL,
    orderdate date NOT NULL,
    requireddate date NOT NULL,
    shippeddate date,
    status character varying(15) NOT NULL,
    comments text,
    customernumber integer NOT NULL
);

CREATE TABLE public.payments (
    customernumber integer NOT NULL,
    checknumber character varying(50) NOT NULL,
    paymentdate date NOT NULL,
    amount numeric(10,2) NOT NULL
);

CREATE TABLE public.productlines (
    productline character varying(50) NOT NULL,
    textdescription character varying(4000) DEFAULT NULL::character varying,
    htmldescription character varying(4000),
    image character varying
);

CREATE TABLE public.products (
    productcode character varying(15) NOT NULL,
    productname character varying(70) NOT NULL,
    productline character varying(50) NOT NULL,
    productscale character varying(10) NOT NULL,
    productvendor character varying(50) NOT NULL,
    productdescription text NOT NULL,
    quantityinstock smallint NOT NULL,
    buyprice numeric(10,2) NOT NULL,
    msrp numeric(10,2) NOT NULL
);
