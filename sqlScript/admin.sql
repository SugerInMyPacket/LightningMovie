use lightning;

drop table if exists adminInfo;

create table adminInfo(
    adminNumber int unsigned not null auto_increment,
    adminName nvarchar(100),
    adminSex char(1) default 'w' not null,
    adminPhone char(11) not null,
    adminPass nvarchar(100) not null,
    constraint pk_admin primary key (adminNumber),
    constraint unique_adminPhone unique (adminPhone)
);

insert into adminInfo values(20171742, "郭俊帅", 'm','15730343165','qwert');

select * from adminInfo;

