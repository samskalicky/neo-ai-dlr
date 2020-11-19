#ifndef DLR_TVM_H_
#define DLR_TVM_H_

#include <graph/graph_runtime.h>
#include <tvm/runtime/memory.h>
#include <tvm/runtime/registry.h>

#include "dlr_common.h"

#if defined(_MSC_VER) || defined(_WIN32)
#define DLR_DLL __declspec(dllexport)
#else
#define DLR_DLL
#endif  // defined(_MSC_VER) || defined(_WIN32)

namespace dlr {

/*! \brief class TVMModel
 */
class DLR_DLL TVMModel : public DLRModel {
 private:
  tvm::runtime::ObjectPtr<tvm::runtime::GraphRuntime> tvm_graph_runtime_;
  std::shared_ptr<tvm::runtime::Module> tvm_module_;
  std::vector<const DLTensor*> outputs_;
  std::vector<std::string> output_types_;
  std::vector<std::string> weight_names_;
  ModelPath GetModelPath(const std::vector<std::string>& files);
  void SetupTVMModule(const std::vector<std::string>& files);
  void SetupTVMModule(const std::vector<DLRModelElem>& model_elems);
  void SetupTVMModule(const std::string& model_lib, const std::string& json_str,
                      dmlc::Stream* param_strm);

  void UpdateInputShapes();

 public:
  /*! \brief Load model files from given folder path.
   */
  explicit TVMModel(const std::vector<std::string>& files, const DLContext& ctx)
      : DLRModel(ctx, DLRBackend::kTVM) {
    SetupTVMModule(files);
  }
  explicit TVMModel(std::vector<DLRModelElem> model_elems, const DLContext& ctx)
      : DLRModel(ctx, DLRBackend::kTVM) {
    SetupTVMModule(model_elems);
  }

  virtual const int GetInputDim(int index) const override;
  virtual const int64_t GetInputSize(int index) const override;
  virtual const char* GetInputName(int index) const override;
  virtual const char* GetInputType(int index) const override;
  virtual void GetInput(const char* name, void* input) override;
  virtual void SetInput(const char* name, const int64_t* shape, const void* input,
                        int dim) override;
  void SetInputTensor(const char* name, DLTensor* tensor);

  virtual void GetOutput(int index, void* out) override;
  void GetOutputManagedTensorPtr(int index, const DLManagedTensor** out);
  virtual const void* GetOutputPtr(int index) const override;
  virtual void GetOutputShape(int index, int64_t* shape) const override;
  virtual void GetOutputSizeDim(int index, int64_t* size, int* dim) override;
  virtual const char* GetOutputType(int index) const override;
  void GetOutputTensor(int index, DLTensor* out);

  virtual const char* GetWeightName(int index) const override;
  virtual std::vector<std::string> GetWeightNames() const override;

  virtual void Run() override;
  virtual DLRBackend GetBackend() const override;
  virtual void SetNumThreads(int threads) override;
  virtual void UseCPUAffinity(bool use) override;

  /*
    Following methods use metadata file to lookup input and output names.
  */
  virtual const char* GetOutputName(const int index) const override;
  virtual int GetOutputIndex(const char* name) const override;
  virtual void GetOutputByName(const char* name, void* out) override;
};

}  // namespace dlr

#endif  // DLR_TVM_H_
