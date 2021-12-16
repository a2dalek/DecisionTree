# Decision Tree for Titanic DataSet
## I. Giới thiệu dữ liệu
### 1. Cấu trúc của dữ liệu:
Dữ liệu của một người bao gồm các thuộc tính:
* PassengerId: số thứ tự.
* Pclass: Loại vé của người đó. Vé sẽ gồm 3 loại 1, 2 và 3.
* Name: Tên kèm danh xưng (Mrs, Mr, Miss,...).
* Age: Tuổi.
* Sex: Giới tính, "male" hoặc "female".
* SibSp: Tổng số anh chị em.
* Parch: Tổng số con cái, bố, mẹ.
* Ticket: Mã vé.
* Fare: Giá vé.
* Cabin: Mã phòng.
* Embarked: Cảng nơi người đó khởi hành, gồm 3 cảng là C, S, Q.
* **Survived**: nhãn cần dự đoán, 0 tức là chết, 1 là sống.
### 2. Các file dữ liệu:
* File "train.csv" gồm dữ liệu của 891 người, sử dụng cho việc xây dựng decision tree.
* File "test.csv" gồm dữ liệu của 418 người, cần dự đoán nhãn. (Trong file đã được bổ sung cột Survived để hợp với hàm đọc dữ liệu).
## II. Cấu trúc chương trình
* Các tham số quan trọng:
    * numberOfAttributes: số thuộc tính
    * numberOfLabel: số nhãn
    * pathToTrainSet, pathtoPrivateTestSet: đường dẫn đến các tập dữ liệu
    * MaxDepth: độ sâu tối đa của cây
    * MinSize: kích thước tối thiểu của một nút
* struct Data: chứa các thuộc tính, nhãn, hàm in và các hàm khởi tạo dữ liệu tương ứng (các nhãn và các thuộc tính chưa biết được đặt giá trị là -1)
* struct Node: chứa thuộc tính được chọn, giá trị được chọn, độ sâu của nút, nút trái và nút phải
* struct Tree: chứa nút gốc và hàm in cây
* long long Rand(long long l, long long h): Random ra một số nguyên trong khoảng l đến h
* int toInt(string s), double toDouble(string s): Đổi một xâu sang định dạng số cần thiết
* vector<Data*>* readFromFile(string path): Đọc dữ liệu từ file csv. Trả về vector dữ liệu
* detectLabel(vector<Data*>* dataSet): Xác định nhãn chính của tập dữ liệu 
* calculateGiniScore(vector<Data*>* dataSet): tính gini score của tập dữ liệu
* double calculateAccuracy(vector<int>* predictLabels, vector<Data*>* dataSet): Tính accuracy của dự đoán so với dữ liệu
* vector<vector<int> > constructConfusionMatrix(vector<int>* predictLabels, vector<Data*>* dataSet): Tạo confusion matrix
* void calculateF1Score(vector<vector<int> > confusionMatrix): Tính F1 score của từng nhãn
* void split(Node* node, vector<Data*>* dataSet, int maxDepth, int minSize): thử chia một nút thành 2 nút con
* void delNode(Node* node), void delTree(Tree* tree): xóa nút và xóa cây, giảm thiểu thất thoát bộ nhớ, giảm lỗi bad_alloc();
* Tree* buildTree(vector<Data*>* dataSet, int maxDepth, int minSize): build cây với maxDepth, minSize
* int doPredict(Data* currentData, Node* node): đoán nhãn của một người
* vector<int>* getPredictVector(vector<Data*>* dataSet, Tree* tree): đoán nhãn của một tập dữ liệu
* Tree* buildBestTree(vector<Data*>* trainSet, vector<Data*>* validSet): chọn cây tốt nhất bằng cách duyệt qua khoảng MaxDepth, MinSize
* void randomForrest(int ntree, vector<Data*>* trainSet, vector<Data*>* validSet, vector<Data*>* testSet, vector<Data*>* privateSet): thực hiện random forrest
* void writeToFile(string path, vector<int>* predict): in kết quả dự đoán ra định dạng thích hợp để submit trên kaggle
* void constructValueSetForAttribute(vector<Data*>* dataSet): khởi tạo tập giá trị cho các thuộc tính
* vector<Data*>* spilitTrainSet(vector<Data*>* trainSet): chia tập dữ liệu theo tỉ lệ 4:1

Chương trình hiện tại sẽ chạy random forrest, kết quả dự đoán sẽ được in ra file "predict.csv" (có thể submit trên kaggle), đồng thời in ra màn hình độ chính xác, F1 score của các nhãn đối với tập train và tập test thử.

## III. Các bước phát triển chương trình
### Cơ bản:
Các thuộc tính được lựa chọn ban đầu gồm Pclass, Sex, Age.

Có 177 người bị thiếu thuộc tính Age ở file train (chiếm 19.8%): Loại bỏ.
Có 86 người bị thiếu thuộc tính Age ở file test (chiếm 20.5%): Random tuổi.

Chia train set ra làm 2 tập train và test với tỉ lệ 4:1. Tiếp tục chia tập train thành train và valid với tỉ lệ 4:1.

Xây dựng cây với MaxDepth, MinSize tự nhập: Accuracy trên valid set là 58.6%, trên test set là 60.2%.
### Cải tiến 1:
Thêm Hyperparameter Search: chạy với các giá trị MaxDepth, MinSize.
Accuracy trên valid set là: 76.7%, tren test set là 75.0%.
### Cải tiến 2:
Thêm random forrest.
Accuracy biến thiên khá lớn, cao nhất trên valid set đạt 82.7%, trên test set đạt 83.5%.
### Cải tiến 3:
Thu gọn khoảng của thuộc tính Age: Chỉ giữ lại 3 khoảng (Age<=15 biểu thị cho trẻ nhỏ, 15<Age<=60 biểu thị cho lứa tuổi trung bình, 60<= Age biểu thị cho tuổi già).

Tạo vector giá trị cho từng thuộc tính. Sau đó thay vì for từng thuộc tính của từng người thì sẽ for trên vector giá trị đó.

Thời gian chạy của thuật toán giảm khoảng 200 lần
### Cải tiến 4:
Xử lý missing data: Gán cho những người bị thiếu tuổi độ tuổi trung bình hoặc già.

Accuracy trên test valid set đạt khoảng 78.6%, trên test set đạt khoảng 80.8%. Accuracy giảm khi train nhưng lại tăng khi submit tập test ẩn.
### Cải tiến 5:
Bổ sung các thuộc tính SibSp, Parch, Embarked.

Có 2 người bị thiếu thuộc tính Embarked: Random thuộc tính này.

Accuracy trên test valid set đạt khoảng 80.5%, trên test set đạt khoảng 84.6%.
### Cải tiến 6:
Xử lý mất cân bằng nhãn: nhãn 0 đang chiếm 61.6%, nhãn 1 đang chiếm 38.4%. Chỉnh sửa bước random trong random forrest để nhãn 0 chiếm 55%, nhãn 1 chiếm 45%.

Accuracy tối đa trên test set tăng khoảng 2%.
### Cải tiến 7: 
Thêm tính F1 score cho các nhãn và hàm in cây.

### Các vấn đề còn lại:
* Underfitting: Xảy ra trong khi train, accuracy trên valid set thường thấp hơn trên test set khoảng 8-10%
* Lỗi std:bad_alloc: Xảy ra khi chương trình xây dựng khoảng 10000 cây độ cao 1-10, hoặc khi xây dựng khoảng 6000 cây độ cao 5-15. Theo tra cứu thì lỗi này xảy ra khi không đủ bộ nhớ, mặc dù đã có hàm delNode(), delTree() và hủy cấp phát những con trỏ ngay khi không dùng tới nữa để giảm khả năng xảy ra lỗi này.

## IV. Kết quả cuối cùng
Accuracy tối đa khi submit trên kaggle là 79.186%







