# Faster Zhang-Suen Thinning Algorithm

This repository provides an optimized implementation of the Zhang-Suen thinning algorithm for skeleton extraction. It is adapted from the `skimage.morphology` module in the [scikit-image](https://scikit-image.org/) library and is designed to be a high-performance alternative to the `cv::ximgproc::thinning` function in OpenCV. The implementation is tailored for seamless integration and ease of use in C++ projects.

## Performance Comparison

Benchmark tests on an Intel i7-8700k processor:

- **Faster Thinning**: 0.08632 seconds per run  
- **OpenCV `cv::ximgproc::thinning`**: 0.27614 seconds per run

Benchmark tests on an Intel i5-10400 processor:

- **Faster Thinning**: 0.02880 seconds per run  
- **OpenCV `cv::ximgproc::thinning`**: 0.24943 seconds per run

## Seamless Integration

This implementation is designed to be a drop-in replacement for `cv::ximgproc::thinning`. You can easily integrate it into your existing C++ projects without modifying your core logic. Simply import the provided code and use it as a direct substitute.

## Key Features

- **High Performance**: Achieves faster processing times without compromising the quality of the skeleton extraction.
- **Ease of Integration**: Compatible with existing OpenCV-based projects, ensuring minimal disruption to your workflow.
- **Reliability**: Based on the well-established Zhang-Suen thinning algorithm, ensuring accurate and consistent results.
- **Proven Implementation**: Adapted from the widely-used and well-regarded `scikit-image` library.
- **C++ Friendly**: Specifically optimized for C++ usage, with clear and concise code structure.

## License
This project is licensed under the MIT License.
