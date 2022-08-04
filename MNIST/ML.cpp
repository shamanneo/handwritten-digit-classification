#include "pch.h"
#include "ML.h"

CML::CML()
	: m_model(nullptr), m_session(nullptr), m_binding(nullptr), m_imageFrame(nullptr)
{
	m_modelPath = L"C:\\Programming\\Win32-programming\\Windows-Machine-Learning-master\\SharedContent\\models\\mnist.onnx" ; 
	m_deviceName = "default" ;
	m_imagePath = L"C:\\Projects\\Handwritten-Digit-Classification\\MNIST\\input.png" ; 
	m_deviceKind = LearningModelDeviceKind::Default ;
	m_labelsFilePath = "C:\\Programming\\Win32-programming\\Windows-Machine-Learning-master\\Samples\\MNIST\\Labels.txt" ;   
}

CML::~CML()
{

}

void CML::LoadModel()
{
    // Load the model 
    ATLTRACE("Loading modelfile '%ws' on the '%s' device\n", m_modelPath.c_str(), m_deviceName.c_str()) ;
    DWORD ticks = ::GetTickCount() ; 
    m_model = LearningModel::LoadFromFilePath(m_modelPath) ; 
    ticks = ::GetTickCount() - ticks ; 
    ATLTRACE("model file loaded in %d ticks\n", ticks) ;
}

void CML::LoadImageFile() 
{
    ATLTRACE("Loading the image...\n") ;
    DWORD ticks = ::GetTickCount() ;
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
    ticks = ::GetTickCount() - ticks ;
    ATLTRACE("image file loaded in %d ticks\n", ticks) ;
    // all done
    m_imageFrame = inputImage ; 
}

void CML::BindModel()
{
    ATLTRACE("Binding the model...\n") ; 
    DWORD ticks = GetTickCount() ;

    // now create a session and binding
    m_session = LearningModelSession{ m_model, LearningModelDevice(m_deviceKind) } ;
    m_binding = LearningModelBinding{ m_session } ;
    // bind the intput image
    m_binding.Bind(L"Input3", ImageFeatureValue::CreateFromVideoFrame(m_imageFrame)) ;
    // bind the output
    std::vector<int64_t> shape({ 1, 1000, 1, 1 }) ;
    m_binding.Bind(L"Plus214_Output_0", TensorFloat::Create(shape)) ;

    ticks = GetTickCount() - ticks ;
    ATLTRACE("Model bound in %d ticks\n", ticks) ;
}

void CML::EvaluateModel(std::string &pred)
{
    // now run the model
    ATLTRACE("Running the model...\n") ;
    DWORD ticks = GetTickCount() ;

    auto results = m_session.Evaluate(m_binding, L"RunId") ;

    ticks = GetTickCount() - ticks ;
    ATLTRACE("model run took %d ticks\n", ticks) ;

    // get the output
    auto resultTensor = results.Outputs().Lookup(L"Plus214_Output_0").as<TensorFloat>() ;
    auto resultVector = resultTensor.GetAsVectorView() ;
    GetResults(resultVector, pred) ;
}

void CML::GetResults(IVectorView<float> results, std::string &pred) 
{
    // load the labels
    LoadLabels() ;
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

void CML::LoadLabels()
{
    // Parse labels from labels file.  We know the file's entries are already sorted in order.
    std::ifstream labelFile { m_labelsFilePath, std::ifstream::in } ;  
    if (labelFile.fail())
    {
        ATLTRACE("failed to load the %s file. Make sure it exists in the same folder as the app\r\n", m_labelsFilePath.c_str()) ;
        exit(EXIT_FAILURE) ;
    }

    std::string s;
    while (std::getline(labelFile, s, ','))
    {
        int labelValue = atoi(s.c_str()) ;
        if (labelValue >= m_labels.size())
        {
            m_labels.resize(labelValue + 1) ;
        }
        std::getline(labelFile, s) ;
        m_labels[labelValue] = s ;
    }
}





