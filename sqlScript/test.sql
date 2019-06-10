use lightning;
-- alter table ticket drop column price;

-- alter table  stage 
-- add price int unsigned not null;

set global log_bin_trust_function_creators = 1;
drop function getkey;
delimiter //
create function getKey(tableName nvarchar(100))
returns nvarchar(100)
begin
declare _key nvarchar(100);
select group_concat(COLUMN_NAME SEPARATOR ',') into _key
  from information_schema.'key_column_usage'
    where table_name= tableName 
        and CONSTRAINT_SCHEMA='lightning'
        and constraint_name='primary';
return (_key);
end
//
delimiter ;

select getKey("movielabel");
 