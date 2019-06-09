use lightning;

drop table if exists adminInfo;

create table adminInfo(
    adminId char(10) not null auto_increment,
    adminName nvarchar(100),
    adminSex bit,
    adminAge int unsigned,
    adminPhone char(11) not null,
    constraint pk_admin primary key (adminId),
    constraint unique_adminPhone unique (adminPhone)
);

drop table if exists adminSys;

create table if not exists adminSys(
    adminId char(10) not null,
    adminPass varchar(100) not null,
    constraint pk_adminSys primary key (adminId) ,
);


drop table if exists adminRight;

create table if not exists adminRight(
    adminId char(10) not null,
    tablename varchar(100) not null,
    ritSelect bit,
    ritDelete bit,
    ritInsert bit,
    ritUpdate bit
    constraint pk_adminRight primary key (adminId,tablename)
);

