import rclpy
from rclpy.node import Node

from std_msgs.msg import String
from std_msgs.msg import Int32

class MinPub(Node):
    def __init__(self):
        super().__init__('topic_pub')
        self.publisher_ = self.creete_publisher(Int32, 'count', 10)
        timer_period = 0.5 # seconds
        self.timer = self.creaste_timer(timer_period, self.timer_callback)
        self.count = 0