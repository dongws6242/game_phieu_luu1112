# Bài tập lớn lập trình nâng cao - INT2215 1

- Họ và tên:NGUYỄN HỮU CỨ
- MSV: 22026532
- Lớp: K67 J

# Giới thiệu Game 
- Tên game :Battle in the labyrinth.
- Thể loại : Khám Phá , Phiêu lưu.


## Ý Tưởng
- 1. Muốn tạo một con game mà trong đó nhân vật của mình sẽ đi khám phá, phiêu lưu trên những chặng đường. Từ đó tạo ra một cái mê cung đơn gian giản cho nhân vật đi vào .
- 2. Sau khi tạo xong mê cung , tạo ra những vật phẩm mà nhân vật có thể ăn được từ đó tăng thêm tiền (điểm ) cho người chơi .
- 3. Tạo ra một con Boss mà người chơi sẽ phải đối đầu để qua màn thông qua trò chơi Kéo - Búa - Bao.
- Tóm lại , ý tưởng của Game là sẽ đưa người chơi đi phiêu lưu và thám hiểm . Trong cuộc hành trình đó : Người chơi phải khéo léo để vượt qua những chướng ngại vật cũng như là những thử thách để nhân vật có thể qua màn.

## Vì sao chọn game
-  Muốn phiêu lưu thông qua con game mà mình tự nghĩ ra.
- Muốn tạo ra một sản phẩm đầu tay mà mình bỏ công suy nghĩ làm từ mọi công đoạn :
    - Lên ý tưởng .
    - Bắt tay vào làm .
    - Hoàn thành Game.
- Game đơn giản không quá cầu kì và qua đó hiểu được các câu lệnh trong SDL2 - C++ mà mình dùng trong game.

## Cách chạy chương trình 
- Hiện tại Windows là Hệ điều hành phổ biến cũng như là phù hợp với giá cả , Vì vậy nên  Mình sẽ chỉ cách chạy trên Windows.
    - 1, Đầu tiên để Game có thể chạy Bạn phải cài trình biên dịch để cho máy tính của bạn có thể hiểu và chạy  chương trình thông qua  trang web : https://codecute.com/c/huong-dan-cai-dat-trinh-bien-dich-c-c-mingw-gcc.html
     Chú ý : xem Hệ điều hành của máy bạn phù hợp với phiên bản nào !
    - 2, Sau khi cài được GCC trên Windows bạn hãy tìm và cài đặt  Visual Studio Code và SDL2.
    Bạn có thể tham khảo : https://www.youtube.com/watch?v=Zdg6aaBTctw

    - 3, Sau khi cài đặt được VS code và SDL2 thì trong bạn cần tải thêm các thư viện của SDL2 như : SDL2_image,SDL2_mixer,SDL2_ttf trên trang : https://www.libsdl.org/projects

    - 4, Sau khi cài đặt đầy đủ các thư viện và cài các tệp của tôi thì bạn Vào chương trình chính : Adventures.cpp 
        - B2 : trên Thanh công cụ Terminal bạn chọn : Run Build Task (hoặc Ctrl + Shift + B) để chạy chương trình.
        - B3 : Nếu xuất hiện "Build finished successfully. " nghĩa là bạn đã chạy được chương trình.
        - B4 : Terminal / New Terminal  khi đó ở phía dưới sẽ xuất hiện đường dẫn để bạn nhập ".\Adventures.exe" . Sau đó nhấn OK và chơi thôi

## Các chức năng chính 
 - Khi vào chơi game bạn cần : 
        - Nhấn nút " Lên ", "Xuống", "Trái", "Phải" để di chuyển nhân vật đi lên, đi xuống, sang trái, sang phải.

        -  Hãy để ý những bức tường đỏ : Nếu như va chạm bạn sẽ chết và thoát ra khỏi chương trình.

        - Hãy cố gắng để ăn các Mục tiêu để ra tăng điểm số . Lưu í có cả những mục tiêu trừ điểm.

        - Hãy di chuyển một cách khôn ngoan để có thể ăn các mục tiêu.

        - Hãy đi đến góc cuối chương trình để bạn có thể đánh Boss . Tùy vào mỗi level mà Boss của bạn nằm ở góc trái hay góc phải. Và khi diệt Boss bạn sẽ có những lợi ích rất cao.

        - Để tắt Nhạc nền bạn hãy nhấn phím "P" trên bàn phím để tắt âm thanh.

## Đề xuất hướng cái thiện

- 1.Tối ưu các bước di chuyển, Hình ảnh sao cho người chơi cảm nhận rõ là nhân vật đã va chạm bằng hình ảnh chứ không phải tọa độ như hiện tại.

- 2. Cố gắng tạo ra mê cung bằng cách ngẫu nhiên sao cho không tốn công vẽ các bức tường mê cung như hiện tại.

- 3. Thêm các tính năng để nhân vật có thể đi chéo thay vì chỉ đi như hiện tại. 

- 4. Khi mà tiêu diệt Boss , Thêm vào âm thanh và hình ảnh để cho game sinh động hơn .

- 5. Tạo ra các  lợi ích khi tiêu diệt  Boss : Thêm mạng, Thêm tốc độ , Thêm khiên ,....

- 6. Thêm Nút Reset lại game để người chơi có trải nghiệm đầy đủ hơn.

- 7. Tách  chương trình chính ra thành các File Header cho người đọc dễ hiểu hơn  và tối ưu các Code hiện tại để chương trình được tối ưu hơn .

## Các kĩ thuật được sử dụng:
- Sử dụng thư viện SDL, SDL_image, SDL_ttf, SDL_mixer.
- Kĩ thuật tách file.
## Kết luận:
- Tích cực: 
    - Đối với mình :  nó là một thứ của bản thân làm ra , dù xấu dù đẹp có hay hay không hay thì nó cũng đem lại cho mình những niềm vui khi làm ra con game
    - Đối với mọi người : Hãy chơi và cảm nhận nhé <3.
- Tiêu cực:
    - Đối với mình :  Thi thoảng khi không fix được lỗi hay không theo ý mình thì hơi stress một xíu cộng thêm lúc test game mà không đi đến được chỗ mình cần test thì cũng khá bực mình.
    - Đối với mọi người : Hãy chơi và cảm nhận nhé <3.
- Kinh nghiệm và bài học rút ra:
  - Học được cách cài trình biên dịch cùng VS Code , kĩ thuật  tách file, cách dùng SDL và cách dùng GitHub.
  - Rút ra kinh nghiệm quản lí và sắp xếp thời gian, xây dựng kế hoạch.
  - Rèn luyện được khả năng tự học, tự tìm hiểu và khả năng lập trình.

## Tài liệu Tham khảo
### +) CÁC Ý TƯỞNG ĐƯỢC MÌNH THỰC HIỆN VÀ SỬA ĐỔI  SAO CHO PHÙ HỢP VỚI NHỮNG GÌ MÀ TRANG WEB LAZYFOO HƯỚNG DẪN : https://lazyfoo.net/tutorials/SDL/
### +) Cách cài VScode và SDL2 : https://www.youtube.com/watch?v=Zdg6aaBTctw
### +) Nguồn thư viện của SDL2 :https://www.libsdl.org/projects
### +) cách cài trình biên dịch : https://codecute.com/c/huong-dan-cai-dat-trinh-bien-dich-c-c-mingw-gcc.html



