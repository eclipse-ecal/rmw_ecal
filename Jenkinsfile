pipeline {
    agent none

    stages {
        stage('Build') {
            parallel {
                stage('Linux - Eloquent') {
                    agent {
                        dockerfile {
                            additionalBuildArgs "--pull --build-arg ros_distro=eloquent"
                            label 'DockerHost'
                        }
                    }

					stages {
                        stage('Build') {
                            steps {
                                checkout scm
								sh '''
									. /opt/ros/eloquent/setup.sh
									colcon build
								'''
                            }
                        }
                    } 
                }

				stage('Linux - Dashing') {
                    agent {
                        dockerfile {
                            additionalBuildArgs "--pull --build-arg ros_distro=dashing" 
                            label 'DockerHost'
                        }
                    }
					
					stages {
                        stage('Build') {
                            steps {
                                checkout scm
								sh '''
									. /opt/ros/dashing/setup.sh
									colcon build
								'''
                            }
                        }
                    } 
                }
            }
        }
    }
}