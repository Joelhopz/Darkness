# Darkness

Rendering engine and editor

Editor UI uses Qt.

![alt text](https://github.com/Karmiska/Darkness/blob/master/editor.png "Editor screenshot")

<p>Engine description</p>

Fully GPU driven graphics architecture.
GPU based frustum and occlusion culling using cluster culling. (roughly the same as the one presented here http://advances.realtimerendering.com/s2015/aaltonenhaar_siggraph2015_combined_final_footer_220dpi.pdf)

Physically based lighting model (PBR) supporting the most common light types (directional, spot, point).
Also supporting image based lighting.
![alt text](https://github.com/Karmiska/Darkness/blob/master/substance.png "Substance painter materials")
![alt text](https://github.com/Karmiska/Darkness/blob/master/substance_metal.png "Substance painter materials 2")

Temporal Antialiasing.
PCF Shadows.
Screen space ambient occlusion.

Light probes.
![alt text](https://github.com/Karmiska/Darkness/blob/master/light_probe.png "Light probe")

Supports both DX12 and Vulkan backends.

Shader hotreload (both in editor/in game).
Distributed asset processing.

This project is likely never going to be finished nor production quality. It is only a tool for me to study the subject.

Building it, while possible, is going to take considerable setup since only the source is available here. There are numerous external project requirements which are not included here. Reason for this is simply the sheer size of the project.
