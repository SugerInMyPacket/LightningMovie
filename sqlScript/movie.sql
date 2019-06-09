use lightning;

drop table if exists movie;

create table if not exists movie(
	movieId char(15) not null,
	movieName nvarchar(100) not null default 'movie',
	movieDirector nvarchar(100),
	movieStar nvarchar(100),
	constraint pk_movie primary key (movieId),
	constraint unique_movieName unique (movieName)
);

insert into movie values ("1234567890abcdg","霹雳战元史之动机风云","霹雳布袋戏","楼至韦陀");
insert into movie values ("1234567890abcdh","霹雳惊鸿之刀剑春秋","霹雳布袋戏","意琦行/绮罗生");
insert into movie values ("1234567890abcdi","霹雳侠影之轰动武林","霹雳布袋戏","三余无梦生");
insert into movie values ("1234567890abcdj","霹雳侠影之轰定干戈","霹雳布袋戏","鷇音子");
insert into movie values ("1234567890abcdk","霹雳侠影之轰掣天下","霹雳布袋戏","倦收天/原无乡");
insert into movie values ("1234567890abcdl","霹雳侠影之轰霆剑海录","霹雳布袋戏","素还真");
insert into movie values ("1234567890abcdm","霹雳开天记之创神篇","霹雳布袋戏","红冕七元");
insert into movie values ("1234567890abcdn","霹雳开天记之创神篇下阙","霹雳布袋戏","赮毕钵罗/深海主宰");
insert into movie values ("1234567890abcdo", "霹雳惊涛","霹雳布袋戏","意琦行");
insert into movie values ("1234567890abcdp","霹雳魔封","霹雳布袋戏","青阳子");

select * from movie;

-- 增加一部电影
delimiter // 
create procedure addMovie(
	in mid char(15),  
	in mName nvarchar(100),  
	in mDirector nvarchar(100),  
	in mStar nvarchar(100) )  
begin
	insert into movie values(mid,mName,mDirector,mStar); 
end
 // 
delimiter ; 

-- 删除一部电影
delimiter //
create procedure removeMovie(
	in mName nvarchar(100))
begin
	delete from movie where movieName = mName;
end //
delimiter ;

-- 更新导演
delimiter //
create procedure modifyDirector(
	in mName nvarchar(100),
	in mDirector nvarchar(100))
begin
	update movie set movieDirector = mDirector 
	where movieName = mName; 
end//
delimiter ;


-- 修改主演
delimiter //
create procedure modifyStar(
	in mName nvarchar(100),
	in mStar nvarchar(100))
begin
	update movie set movieStar = mStar
	where movieName = mName;
end//
delimiter ;

-- 修改电影名称
delimiter //
create procedure modifyMovie(
	in oldName nvarchar(100),
	in new_name nvarchar(100))
begin
	update movie set movieName = new_name
	where movieName = oldName;
end//
delimiter ;

-- 获取电影id
delimiter //
create function getMovieId(
	mName nvarchar(100)) returns char(15)
begin
	declare id char(15);
	select movieId into id from movie 
	where movieName = mName;
	return id;
end//
delimiter ;

