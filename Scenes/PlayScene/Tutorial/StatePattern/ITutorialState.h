#pragma once

// �O���錾
class TutorialStateContext;
class ITutorialState
{
protected:
    TutorialStateContext* m_tutorialContext;

public:
    virtual ~ITutorialState() {}

    // �`���[�g���A���R���e�L�X�g���Z�b�g����
    void Set_Context(TutorialStateContext* context) {this->m_tutorialContext = context; }

    virtual void Update() = 0;
};

class TutorialStateContext
{

private:
    ITutorialState* m_Tutorialstate;

public:
    TutorialStateContext(ITutorialState* state) : m_Tutorialstate(nullptr) { this->TransitionTo(state); }
    ~TutorialStateContext() { delete m_Tutorialstate;}

    // ���̃X�e�[�g�Ɉڍs����
    void TransitionTo(ITutorialState* state) 
    {
        // ��������Ă���ꍇ�Ɍ���s��
        if (this->m_Tutorialstate != nullptr)
         
        // ���݂̃X�e�[�g������
        delete this->m_Tutorialstate;

        // �V�����X�e�[�g�ɓ���ւ���
        this->m_Tutorialstate = state;
        this->m_Tutorialstate->Set_Context(this);
    }

    void Update() { this->m_Tutorialstate->Update(); }
};