#ifndef ILRD_HRD42_ICOMMAND_HPP
#define ILRD_HRD42_ICOMMAND_HPP

namespace hrd42
{
class ICommand
{
public:
  enum Priority
  {
    LOW = 0,
    MID = 1,
    HIGH = 2,
    ADMIN = 3
  };

  explicit ICommand(Priority priority = MID);
  ICommand(const ICommand& other) = default;
  ICommand& operator=(const ICommand& other) = default;
  virtual ~ICommand() = default;

  virtual bool operator<(const ICommand& comd) const;
  virtual void Execute() = 0;

private:
  Priority m_priority;
};
} // namespace hrd42

#endif // ILRD_HRD42_ICOMMAND_HPP