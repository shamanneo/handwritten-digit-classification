#include "pch.h"
#include "ML.h"

CML::CML()
	: m_model(nullptr), 
      m_session(nullptr), 
      m_binding(nullptr), 
      m_imageFrame(nullptr)
{
	m_modelPath = L"C:\\Programming\\Win32-programming\\Windows-Machine-Learning-master\\SharedContent\\models\\mnist.onnx" ; 
	m_deviceName = "default" ;
	m_imagePath = L"C:\\Projects\\Handwritten-Digit-Classification\\MNIST\\input.png" ; 
	m_deviceKind = LearningModelDeviceKind::Default ;
    std::vector<std::string> tempLabel { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" } ; 
    m_labels = tempLabel ; 
}

CML::~CML()
{

}

void CML::LoadModel()
{
    // Load the model 
    m_model = LearningModel::LoadFromFilePath(m_modelPath) ; 
}

void CML::LoadImageFile() 
{
    VideoFrame inputImage = nullptr ;
    try
    {
        // open the file
        StorageFile file = StorageFile::GetFileFromPathAsync(m_imagePath).get() ;
        // get a stream on it
        auto stream = file.OpenAsync(FileAccessMode::Read).get() ;
        // Create the decoder from the stream
        BitmapDecoder decoder = BitmapDecoder::CreateAsync(stream).get() ;
        // get the bitmap
        SoftwareBitmap softwareBitmap = decoder.GetSoftwareBitmapAsync().get() ;
        // load a videoframe from it
        inputImage = VideoFrame::CreateWithSoftwareBitmap(softwareBitmap) ;
    }
    catch (...)
    {
        ATLTRACE("failed to load the image file, make sure you are using fully qualified paths\r\n") ;
        exit(EXIT_FAILURE) ;
    }
    // all done
    m_imageFrame = inputImage ; 
}

void CML::BindModel()
{
    // now create a session and binding
    m_session = LearningModelSession{ m_model, LearningModelDevice(m_deviceKind) } ;
    m_binding = LearningModelBinding{ m_session } ;
    // bind the intput image
    m_binding.Bind(L"Input3", ImageFeatureValue::CreateFromVideoFrame(m_imageFrame)) ;
    // bind the output
    // std::vector<int64_t> shape({ 1, 1000, 1, 1 }) ;
    std::vector<int64_t> shape({ 1, 10 }) ;
    m_binding.Bind(L"Plus214_Output_0", TensorFloat::Create(shape)) ;
}

void CML::EvaluateModel(std::string &pred)
{
    // now run the model
    auto results = m_session.Evaluate(m_binding, L"RunId") ;

    // get the output
    auto resultTensor = results.Outputs().Lookup(L"Plus214_Output_0").as<TensorFloat>() ;
    auto resultVector = resultTensor.GetAsVectorView() ;
    GetResults(resultVector, pred) ;
}

void CML::GetResults(IVectorView<float> results, std::string &pred) 
{
    // Find the top 3 probabilities
    std::vector<float> topProbabilities(3) ;
    std::vector<int> topProbabilityLabelIndexes(3) ;
    for (uint32_t i = 0; i < results.Size(); i++)
    {
        // is it one of the top 3?
        for (int j = 0; j < 3; j++)
        {
            if (results.GetAt(i) > topProbabilities[j])
            {
                topProbabilityLabelIndexes[j] = i ;
                topProbabilities[j] = results.GetAt(i) ;
                break ;
            }
        }
    }
    // Model prediction.
    pred = m_labels[topProbabilityLabelIndexes[0]].c_str() ; 
}







