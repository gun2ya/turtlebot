# TurtleBot3 사용법

## 1. TurtleBot 접근방법


```bash
$ ssh koreatechcse02@[ip address]
$ ssh koreatechcse02@192.168.0.159
```

## 2. Turtlebot Bringup

```bash
$ ros2 launch turtlebot3_bringup robot.launch.py
```

## 3. Turtlebot camera

카메라 권한주기
```bash
$ sudo usermod -aG video $USER 
#이거 실행 후 reboot 
```
카메라 노드 실행
```bash
$ ros2 run v4l2_camera v4l2_camera_node
```


