
pipeline {
    agent{
        label "linux&&clang-17"
    }

    parameters {
        string(name: 'PLATFORM', defaultValue: 'linux', description: 'Platform to build on')
        string(name: 'COMPILER', defaultValue: 'clang', description: 'Compiler to use')
        string(name: 'BUILD_TYPE', defaultValue: 'Release', description: 'Build type')
    }

    environment {
        SONARQUBE_URL = 'https://sonarqube.shodan.fyi'
    }

    stages {
        /*
        stage('Retrieving Artifacts') {
            steps {
                script{
                    try {
                        unarchive (mapping: [
                            "build-${params.PLATFORM}-${params.COMPILER}/": "build"
                        ])
                        artifactsRetrieved = true
                    } catch (Exception e) {

                    }
                }
            }
        }
        */
        stage('initialize submodules') {
            steps {
                script {
                    sh 'git submodule update --init --recursive'
                }
            }
        }
        stage('Prepare Tools'){
            steps {
                script {
/*/                    if(COMPILER == 'clang') {
                        sh 'apt install -y llvm'
                    }
                    sh "apt install -y python3-venv"
*/
                    sh "apt install -y lcov doxygen"
                    sh """python3 -m venv venv
                        . venv/bin/activate
                        pip install lcov_cobertura
                    """
                }
            }
        }
        stage('Build') {
            steps {
                sh """
                cmake -B build/ -DCMAKE_BUILD_TYPE=${params.BUILD_TYPE} -DKestrelPack_BUILD_DOCS=OFF
                cmake --build build/ --config=${params.BUILD_TYPE} -j
                """
            }
        }
        stage('Creating Test Coverage Reports') {
            steps {
                catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
                    sh """
                    cmake --build build/ --config=${params.BUILD_TYPE} --target coverage

                    . venv/bin/activate
                    # Convert lcov report to cobertura format
                    lcov_cobertura build/coverage.lcov -o coverage.xml
                    """

                    recordCoverage(
                        qualityGates: [
                            [criticality: 'NOTE', integerThreshold: 60, metric: 'FILE', threshold: 60.0],
                            [criticality: 'NOTE', integerThreshold: 60, metric: 'LINE', threshold: 60.0],
                            [criticality: 'NOTE', integerThreshold: 60, metric: 'METHOD', threshold: 60.0]
                        ],
                        tools: [[parser: 'COBERTURA', pattern: 'coverage.xml']])
                }
            }
        }
        stage('Static Analysis') {
            steps {
                catchError(buildResult: 'UNSTABLE', stageResult: 'FAILURE') {
                    sh """
                        apt install -y cppcheck python3-venv
                        python3 -m venv venv
                        . venv/bin/activate
                        pip install lizard
                        cppcheck --enable=all --inconclusive --xml --xml-version=2 -I include/ source/* include/* 2> cppcheck.xml
                        lizard -X "source/*" "include/*" > lizard.xml
                    """

                    recordIssues(
                        sourceCodeRetention: 'LAST_BUILD',
                        tools: [
                            cppCheck(pattern: 'cppcheck.xml'),
                        ]
                    )
                }
            }
        }
        stage('SonarQube Analysis'){
            steps {
                catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
                    script {
                        def scannerHome = tool name: 'SonarScanner', type: 'hudson.plugins.sonar.SonarRunnerInstallation';
                        withSonarQubeEnv() {
                            sh """
                            ${scannerHome}/bin/sonar-scanner -X
                            """
                        }
                    }
                }
            }
        }
        /*
        stage('Archiving Artifacts') {
            steps {
                sh 'mv build "build-${params.PLATFORM}-${params.COMPILER}"'
                archiveArtifacts (artifacts: "build-${params.PLATFORM}-${params.COMPILER}/", allowEmptyArchive: true, onlyIfSuccessful: true, fingerprint: true)
            }
        }
        */
    }
}
