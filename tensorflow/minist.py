import tensorflow as tf
import numpy as np

import input_data
mnist = input_data.read_data_sets('MNIST_data', one_hot = True)

# Tesorflow依赖于一个高校的c++后端来进行计算。与后端的这个连接叫做session。
# 一般而言， 使用tensorflow程序的流程是先创建一个图，然后在session中启动它
# 更方便的InteractiveSession类
# 通过它，可以更加灵活的构建你的代码，
# 能在运行图的时候，插入一些计算图。
# 这些计算图是由某些操作（opertaions）构成的。
# 对于工作在交互式环境中的人们来说非常便利，比如使用Ipython。

# 如果没有使用InteractiveSession,那么你需要在启动session之前构建整个计算图。



sess = tf.InteractiveSession()

# 计算图：
# 1. 为了在python中进行高效的数值计算，将耗时操作在python环境的外部计算， 但是切换操作需要不小的开销， 主要是数据迁移
# 2. Tensorfow同样，为了避免开销，使用的方式是，先让我们描述一个交互操作图，然后完全将其运行在python外部。这与Theano和Torch的做法类似
# 3. 因此python代码是用来构建这个可以在外部运行的计算图，以及安排计算图的哪一部分应该被运行（session）。

# 构建一个拥有一个线性层的softmax回归模型，并扩展为一个拥有多层卷积网络的softmax回归模型。

#占位符
# 1. 以为输入图像和目标输出类别创建节点来构建计算图。

x = tf.placeholder("float", shape=[None, 784]) # None * 784
y_ = tf.placeholder("float", shape=[None, 10]) # None * 10 one-hot向量

#变量

W = tf.Variable(tf.zeros([784, 10])) # 784 * 10
b = tf.Variable(tf.zeros([10]))  # None * 10

sess.run(tf.initialize_all_variables())

#类别预测与损失函数
# 回归模型节点定义：

y = tf.nn.softmax(tf.matmul(x, W) + b)

#为模型指定损失函数： 交叉熵（目标类别和预测类别之间的cross entropy）

cross_entropy = - tf.reduce_sum(y_ * tf.log(y)) #一个mini batch size的所有图片的交叉熵值之和

# 选择训练策略来降低损失函数的值， 达到训练模型的效果: 梯度下降等
# 我们已经定义好模型和训练用的损失函数，那么用TensorFlow进行训练就很简单了。因为TensorFlow知道整个计算图，它可以使用自动微分法找到对于各个变量的损失的梯度值。T

# 这一行代码实际上是用来往计算图上添加一个新操作，其中包括计算梯度，计算每个参数的步长变化，并且计算出新的参数值。
# 返回的train_step操作对象，在运行时会使用梯度下降来更新参数。因此，整个模型的训练可以通过反复地运行train_step来完成。

train_step = tf.train.GradientDescentOptimizer(0.01).minimize(cross_entropy) #指定学习率为0.01 即步长为0.01.

# 在计算图中，你可以用feed_dict来替代任何张量，并不仅限于替换占位符。
for i in range(1000):
  batch = mnist.train.next_batch(50)
  train_step.run(feed_dict={x: batch[0], y_: batch[1]})


#评估模型:
# tf.argmax

correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1)) # None * 1 bool数组

#准确率
#tf.cast强制将bool转为float, 求均值
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float")) 

print(accuracy.eval(feed_dict = {x: mnist.test.images, y_: mnist.test.labels})) # 0.9115