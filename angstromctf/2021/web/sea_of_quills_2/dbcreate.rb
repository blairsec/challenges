require 'sqlite3'

db = SQLite3::Database.new "quills.db"

db.execute <<-SQL
	create table quills (
		url varchar(30),
		name varchar(30),
		desc varchar(30)
	);
SQL


[
	["https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcSDaEvWzHOZApMnm4vWtmTVqkD8Q5Ymq5ASvE_FpAaRMbtrhsiB&usqp=CAU", "pointy", "it is very pointy"],
	["https://img.favpng.com/14/4/5/feather-paper-quill-nib-png-favpng-HCKGfPKS7XJ42HF0Mkv4j2igy.jpg", "this one looks pretty nice too", "it's very special"],
	["https://cdn.shopify.com/s/files/1/0221/1146/products/Brown_Quill_Pen_Rescaled_grande.png?v=1552469210", "awfully realistic", "wouldn't you agree?"],
	["https://prodimage.images-bn.com/pimages/9780765598677_p0_v2_s1200x630.jpg", "in stock", "according to b&n at least"],
	["https://static.turbosquid.com/Preview/2019/07/22__15_53_42/QuillDarkFeatherInkSet3dmodel000.jpg651BF96C-4D0D-4251-A9A1-5334516667D0Large.jpg", "in stock as well", "but this one is a bit more expensive"],
	["https://boyter.org/static/books/CgbR1wkXEAAMjqP.jpg", "wait a minute, this isn't a quill!", "pandaman!"],
].each do |data|
	db.execute "insert into quills values (?, ?, ?)", data
end


db.execute <<-SQL
	create table flagtable (
		flag varchar(30)
	);
SQL


db.execute "insert into flagtable values ('actf{the_time_we_have_spent_together_riding_through_this_english_denylist_c0776ee734497ca81cbd55ea}')"
