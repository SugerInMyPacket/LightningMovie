use lightning;

drop table if exists movieLabel;

create table if not exists movieLabel(
	movId char(15),
	labId nvarchar(15),
	constraint pk_movieLabel primary key (movId, labId),
	constraint fk_mvId foreign key (movId) references movie(movieId),
	constraint fk_labId foreign key (labId) references label(labelId)
);

insert into movieLabel values ("1234567890abcdg","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdh","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdi","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdj","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdk","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdl","霹雳布袋戏");
insert into movieLabel values ("1234567890abcdl", "武侠片");
insert into movieLabel values ("1234567890abcdl","玄幻片");

select * from movieLabel;

------------------------------------------------------------------------------------

delimiter //
create procedure addMovieLabel(
	in mName nvarchar(100),
	in lid nvarchar(15))
begin
	declare mid char(15);
	select movie.movieId into mid from movie where movie.movieName  = mName;
	insert into movieLabel values(mid, lid);
end
//
delimiter ;

delimiter //
create procedure removeMovieLabel(
	in mName nvarchar(100),
	in lid nvarchar(15))
begin
	delete from movieLabel 
	where movId = mid and labId = lid;
end
//
delimiter ;