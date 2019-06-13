
use lightning;

create view movieView(电影,导演,主演) as

select movieName,movieDirector,movieStar from movie;

select * from movieView;