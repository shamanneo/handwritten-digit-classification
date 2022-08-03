#pragma once

using namespace winrt ;
using namespace Windows::Foundation ;

using namespace Windows::AI::MachineLearning ;
using namespace Windows::Foundation::Collections ;
using namespace Windows::Graphics::Imaging ;
using namespace Windows::Media ;
using namespace Windows::Storage ;

class CML
{ 
	private :
		hstring m_modelPath ;
		std::string m_deviceName ;
		hstring m_imagePath ;
		LearningModel m_model ; 
		LearningModelDeviceKind m_deviceKind ; 
		LearningModelSession m_session ; 
		LearningModelBinding m_binding ;
		VideoFrame m_imageFrame ;
		std::string m_labelsFilePath ;
		std::vector<std::string> m_labels ;
	public :
		CML() ; 
		~CML() ; 
	public :
		void LoadModel() ; 
		VideoFrame LoadImageFile() ; 
		void BindModel() ; 
		void EvaluateModel() ; 
		void PrintResults(IVectorView<float> results) ; 
		void LoadLabels() ; 
} ;

