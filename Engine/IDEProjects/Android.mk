 # Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
MY_PATH := $(LOCAL_PATH)/..

include $(MY_PATH)/../Dependencies/LIBPNG/lpng1510/Android.mk
include $(MY_PATH)/../Dependencies/LIBZIP/Android.mk
include $(CLEAR_VARS)
LOCAL_PATH := $(MY_PATH)

LOCAL_MODULE := isdge
LOCAL_CFLAGS := -Werror

# Header search paths
LOCAL_CFLAGS += -I$(MY_PATH)/Common
LOCAL_CFLAGS += -I$(MY_PATH)/Graphics
LOCAL_CFLAGS += -I$(MY_PATH)/Input
LOCAL_CFLAGS += -I$(MY_PATH)/Audio
LOCAL_CFLAGS += -I$(MY_PATH)/Physics
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletDynamics/Character
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletDynamics/Dynamics
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/BulletDynamics/Vehicle
LOCAL_CFLAGS += -I$(MY_PATH)/Physics/Bullet/bullet-2.77/src/LinearMath
LOCAL_CFLAGS += -I$(MY_PATH)/Combiner
LOCAL_CFLAGS += -I$(MY_PATH)/Templates
LOCAL_CFLAGS += -I$(MY_PATH)/OSBindings/Android

LOCAL_CFLAGS += -I$(LOCAL_PATH)/../Dependencies/LIBPNG/lpng1510
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../Dependencies/LIBZIP

# Defines
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CFLAGS += -D__BULLET__
LOCAL_CFLAGS += -D__OUYA__


# COMMON
LOCAL_SRC_FILES := Common/sgBase.cpp
LOCAL_SRC_FILES += Common/sgColor.cpp
LOCAL_SRC_FILES += Common/sgDebug.cpp
LOCAL_SRC_FILES += Common/sgMatrix4x4.cpp
LOCAL_SRC_FILES += Common/sgPlane.cpp
LOCAL_SRC_FILES += Common/sgQuaternion.cpp
LOCAL_SRC_FILES += Common/sgResourceManager.cpp
LOCAL_SRC_FILES += Common/sgTimer.cpp
LOCAL_SRC_FILES += Common/sgTools.cpp
LOCAL_SRC_FILES += Common/sgVector2.cpp
LOCAL_SRC_FILES += Common/sgVector3.cpp
LOCAL_SRC_FILES += Common/sgVector4.cpp
LOCAL_SRC_FILES += Common/sgXML.cpp

# GRAPHICS
LOCAL_SRC_FILES += Graphics/sgCamera.cpp
LOCAL_SRC_FILES += Graphics/sgLight.cpp
LOCAL_SRC_FILES += Graphics/sgMaterial.cpp
LOCAL_SRC_FILES += Graphics/sgMesh.cpp
LOCAL_SRC_FILES += Graphics/sgObject.cpp
LOCAL_SRC_FILES += Graphics/sgObjectBody.cpp
LOCAL_SRC_FILES += Graphics/sgObjectFiles.cpp
LOCAL_SRC_FILES += Graphics/sgPanel.cpp
LOCAL_SRC_FILES += Graphics/sgParticle.cpp
LOCAL_SRC_FILES += Graphics/sgParticleEmitter.cpp
LOCAL_SRC_FILES += Graphics/sgRenderer.cpp
LOCAL_SRC_FILES += Graphics/sgRendererES1.cpp
LOCAL_SRC_FILES += Graphics/sgRendererES2.cpp
LOCAL_SRC_FILES += Graphics/sgRenderEvent.cpp
LOCAL_SRC_FILES += Graphics/sgShader.cpp
LOCAL_SRC_FILES += Graphics/sgShadowVolume.cpp
LOCAL_SRC_FILES += Graphics/sgSkeleton.cpp
LOCAL_SRC_FILES += Graphics/sgTexture.cpp
LOCAL_SRC_FILES += Graphics/sgTextureFiles.cpp

# Input
LOCAL_SRC_FILES += Input/sgAccelerometer.cpp
LOCAL_SRC_FILES += Input/sgCameraStream.cpp
LOCAL_SRC_FILES += Input/sgMouse.cpp
LOCAL_SRC_FILES += Input/sgTouches.cpp
LOCAL_SRC_FILES += Input/sgGamepad.cpp

# Audio
LOCAL_SRC_FILES += Audio/sgAudioFiles.cpp
LOCAL_SRC_FILES += Audio/sgAudioPlayer.cpp
LOCAL_SRC_FILES += Audio/sgSound.cpp
LOCAL_SRC_FILES += Audio/sgSoundSource.cpp

# Physics
LOCAL_SRC_FILES += Physics/sgCharacterController.cpp
LOCAL_SRC_FILES += Physics/sgPhysBody.cpp
LOCAL_SRC_FILES += Physics/sgPhysConstraint.cpp
LOCAL_SRC_FILES += Physics/sgPhysWorld.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btDbvt.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btGhostObject.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/btUnionFind.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btBox2dShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btBoxShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btCollisionShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btCompoundShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConcaveShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConeShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvexShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btCylinderShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btEmptyShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btMultiMaterialTriangleMeshShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btShapeHull.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btSphereShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btContactProcessing.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btGImpactBvh.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btGImpactShape.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/gim_box_set.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/gim_contact.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/gim_memory.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/Gimpact/gim_tri_collision.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Character/btKinematicCharacterController.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Dynamics/btContinuousDynamicsWorld.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Dynamics/btRigidBody.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/BulletDynamics/Vehicle/btWheelInfo.cpp

LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/LinearMath/btAlignedAllocator.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/LinearMath/btConvexHull.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/LinearMath/btGeometryUtil.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/LinearMath/btQuickprof.cpp
LOCAL_SRC_FILES += Physics/Bullet/bullet-2.77/src/LinearMath/btSerializer.cpp

LOCAL_SRC_FILES += Physics/Bullet/sgCharacterControllerBullet.cpp
LOCAL_SRC_FILES += Physics/Bullet/sgPhysBodyBullet.cpp
LOCAL_SRC_FILES += Physics/Bullet/sgPhysConstraintBullet.cpp
LOCAL_SRC_FILES += Physics/Bullet/sgPhysWorldBullet.cpp

# Combiner
LOCAL_SRC_FILES += Combiner/sgAction.cpp
LOCAL_SRC_FILES += Combiner/sgEntity.cpp
LOCAL_SRC_FILES += Combiner/sgEvents.cpp
LOCAL_SRC_FILES += Combiner/sgMain.cpp

# Platform
LOCAL_SRC_FILES += OSBindings/Android/sgInit.cpp

# Templates
LOCAL_SRC_FILES += Templates/CameraFree.cpp
LOCAL_SRC_FILES += Templates/FPSDisplay.cpp
LOCAL_SRC_FILES += Templates/PhysicsObject.cpp
LOCAL_SRC_FILES += Templates/Water.cpp


LOCAL_LDLIBS := -llog -lz -lGLESv2

LOCAL_STATIC_LIBRARIES := libzip libpng

include $(BUILD_STATIC_LIBRARY)
