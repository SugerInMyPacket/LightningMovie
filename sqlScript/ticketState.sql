use lightning;

drop table if exists ticketState;

create table if not exists ticketState(
    tktStateId char(2) not null,
    tktStateDsb nvarchar(100) not null,
    constraint pk_ticketState primary key (tktStateId),
    constraint unique_tktStateDsb unique (tktStateDsb)
);

insert into ticketState values ("ye","已出售(yes)");
insert into ticketState values ("no","未出售(no)");
insert into ticketState values ("ba","退票(back)");

select * from ticketState;

